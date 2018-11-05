
#include "cdl_dac.h"
#include "cdl_sys.h"
#include <bitops.h>
#include "console.h"
/* low-level access */
struct snd_wm8766 g_wm[2];

static void soft_spi_write(struct snd_wm8766 *wm, u16 val) {
	struct ext_ctl_pins *pins = &wm->pins;
	sys_external_pad_output(pins->SW_SPI_CS, 0);
	for (int i=15; i>=0; i--) {
		sys_external_pad_output(pins->SW_SPI_CLK, 0);
		int bit = (val>>i)&0x1;
		sys_external_pad_output(pins->SW_SPI_DAT, bit);
		udelay(1);
		sys_external_pad_output(pins->SW_SPI_CLK, 1);
		udelay(1);
	}
	sys_external_pad_output(pins->SW_SPI_CLK, 0);
	sys_external_pad_output(pins->SW_SPI_CS, 1);
	mdelay(1);
}

void snd_wm8766_soft_spi_init(struct snd_wm8766 *wm, int mode, int cs, int clk, int data) {
	struct ext_ctl_pins *pins = &wm->pins;
	pins->WM8766_MODE_PIN = mode;
	pins->SW_SPI_CS = cs;
	pins->SW_SPI_CLK = clk;
	pins->SW_SPI_DAT = data;
	sys_external_pad_output(pins->SW_SPI_CS, 1);
	sys_external_pad_output(pins->SW_SPI_CLK, 0);
	sys_external_pad_output(pins->SW_SPI_DAT, 0);
}

static void snd_wm8766_write(struct snd_wm8766 *wm, u16 addr, u16 data)
{
	if (addr < WM8766_REG_COUNT)
		wm->regs[addr] = data;
	//wm->ops.write(wm, addr, data);
	u16 val = ((addr&0x7F)<<9) | (data & 0x1FF);
	info("wm8766 wr 0x%x <=> addr 0x%x data 0x%x\n", val, addr, data);
	soft_spi_write(wm, val);
}

/* mixer controls */
static struct snd_wm8766_ctl snd_wm8766_default_ctl[WM8766_CTL_COUNT] = {
	[WM8766_CTL_CH1_VOL] = {
		.name = "Channel 1 Playback Volume",
		.type = SNDRV_CTL_ELEM_TYPE_INTEGER,
		.reg1 = WM8766_REG_DACL1,
		.reg2 = WM8766_REG_DACR1,
		.mask1 = WM8766_VOL_MASK,
		.mask2 = WM8766_VOL_MASK,
		.max = 0xff,
		.flags = WM8766_FLAG_STEREO | WM8766_FLAG_VOL_UPDATE,
	},
	[WM8766_CTL_CH2_VOL] = {
		.name = "Channel 2 Playback Volume",
		.type = SNDRV_CTL_ELEM_TYPE_INTEGER,
		.reg1 = WM8766_REG_DACL2,
		.reg2 = WM8766_REG_DACR2,
		.mask1 = WM8766_VOL_MASK,
		.mask2 = WM8766_VOL_MASK,
		.max = 0xff,
		.flags = WM8766_FLAG_STEREO | WM8766_FLAG_VOL_UPDATE,
	},
	[WM8766_CTL_CH3_VOL] = {
		.name = "Channel 3 Playback Volume",
		.type = SNDRV_CTL_ELEM_TYPE_INTEGER,
		.reg1 = WM8766_REG_DACL3,
		.reg2 = WM8766_REG_DACR3,
		.mask1 = WM8766_VOL_MASK,
		.mask2 = WM8766_VOL_MASK,
		.max = 0xff,
		.flags = WM8766_FLAG_STEREO | WM8766_FLAG_VOL_UPDATE,
	},
	[WM8766_CTL_CH1_SW] = {
		.name = "Channel 1 Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_MUTE1,
		.flags = WM8766_FLAG_INVERT,
	},
	[WM8766_CTL_CH2_SW] = {
		.name = "Channel 2 Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_MUTE2,
		.flags = WM8766_FLAG_INVERT,
	},
	[WM8766_CTL_CH3_SW] = {
		.name = "Channel 3 Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_MUTE3,
		.flags = WM8766_FLAG_INVERT,
	},
	[WM8766_CTL_PHASE1_SW] = {
		.name = "Channel 1 Phase Invert Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_IFCTRL,
		.mask1 = WM8766_PHASE_INVERT1,
	},
	[WM8766_CTL_PHASE2_SW] = {
		.name = "Channel 2 Phase Invert Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_IFCTRL,
		.mask1 = WM8766_PHASE_INVERT2,
	},
	[WM8766_CTL_PHASE3_SW] = {
		.name = "Channel 3 Phase Invert Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_IFCTRL,
		.mask1 = WM8766_PHASE_INVERT3,
	},
	[WM8766_CTL_DEEMPH1_SW] = {
		.name = "Channel 1 Deemphasis Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_DEEMP1,
	},
	[WM8766_CTL_DEEMPH2_SW] = {
		.name = "Channel 2 Deemphasis Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_DEEMP2,
	},
	[WM8766_CTL_DEEMPH3_SW] = {
		.name = "Channel 3 Deemphasis Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_DEEMP3,
	},
	[WM8766_CTL_IZD_SW] = {
		.name = "Infinite Zero Detect Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL1,
		.mask1 = WM8766_DAC_IZD,
	},
	[WM8766_CTL_ZC_SW] = {
		.name = "Zero Cross Detect Playback Switch",
		.type = SNDRV_CTL_ELEM_TYPE_BOOLEAN,
		.reg1 = WM8766_REG_DACCTRL2,
		.mask1 = WM8766_DAC2_ZCD,
		.flags = WM8766_FLAG_INVERT,
	},
};

/* exported functions */

void snd_wm8766_init(struct snd_wm8766 *wm)
{
	int i;
	static const u16 default_values[] = {
		0x000, 0x100,
		0x120, 0x002,
		0x000, 0x100, 0x000, 0x100, 0x000,
		0x000, 0x080,
	};

	memcpy(wm->ctl, snd_wm8766_default_ctl, sizeof(wm->ctl));

	sys_external_pad_output(wm->pins.WM8766_MODE_PIN, 0); /* sw mode */
	snd_wm8766_write(wm, WM8766_REG_RESET, 0x00); /* reset */
	udelay(10);
	/* load defaults */
	for (i = 0; i < ARRAY_SIZE(default_values); i++)
		snd_wm8766_write(wm, i, default_values[i]);
	info("wm8766 reg init done.\n");

    snd_wm8766_build_controls(wm);
}

void snd_wm8766_resume(struct snd_wm8766 *wm)
{
	int i;

	for (i = 0; i < WM8766_REG_COUNT; i++)
		snd_wm8766_write(wm, i, wm->regs[i]);
}

/* e.g. snd_wm8766_set_if(wm, WM8766_IF_FMT_I2S | WM8766_IF_IWL_16BIT) */
void snd_wm8766_set_if(struct snd_wm8766 *wm, u16 dac)
{
	u16 val = wm->regs[WM8766_REG_IFCTRL] & ~WM8766_IF_MASK;

	dac &= WM8766_IF_MASK;
	snd_wm8766_write(wm, WM8766_REG_IFCTRL, val | dac);
}

void snd_wm8766_volume_restore(struct snd_wm8766 *wm)
{
	u16 val = wm->regs[WM8766_REG_DACR1];
	/* restore volume after MCLK stopped */
	snd_wm8766_write(wm, WM8766_REG_DACR1, val | WM8766_VOL_UPDATE);
}

#define lsb_offset(x)  (ffs(x)-1)

static int snd_wm8766_ctl_get(struct snd_wm8766 *wm, int ctl_id, u16 *regval1, u16 *regval2)
{
	u16 val1, val2;

	if (wm->ctl[ctl_id].get)
		wm->ctl[ctl_id].get(wm, &val1, &val2);
	else {
		val1 = wm->regs[wm->ctl[ctl_id].reg1] & wm->ctl[ctl_id].mask1;
		val1 >>= lsb_offset(wm->ctl[ctl_id].mask1);
		if (wm->ctl[ctl_id].flags & WM8766_FLAG_STEREO) {
			val2 = wm->regs[wm->ctl[ctl_id].reg2] & wm->ctl[ctl_id].mask2;
			val2 >>= lsb_offset(wm->ctl[ctl_id].mask2);
			if (wm->ctl[ctl_id].flags & WM8766_FLAG_VOL_UPDATE)
				val2 &= ~WM8766_VOL_UPDATE;
		}
	}
	if (wm->ctl[ctl_id].flags & WM8766_FLAG_INVERT) {
		val1 = wm->ctl[ctl_id].max - (val1 - wm->ctl[ctl_id].min);
		if (wm->ctl[ctl_id].flags & WM8766_FLAG_STEREO)
			val2 = wm->ctl[ctl_id].max - (val2 - wm->ctl[ctl_id].min);
	}
	*regval1 = val1;
	if (wm->ctl[ctl_id].flags & WM8766_FLAG_STEREO)
		*regval2 = val2;

	return 0;
}

static int snd_wm8766_ctl_put(struct snd_wm8766 *wm, int ctl_id, u16 regval1, u16 regval2)
{
	u16 val; //, regval1, regval2;

	/* this also works for enum because value is an union */
	if (wm->ctl[ctl_id].flags & WM8766_FLAG_INVERT) {
		regval1 = wm->ctl[ctl_id].max - (regval1 - wm->ctl[ctl_id].min);
		regval2 = wm->ctl[ctl_id].max - (regval2 - wm->ctl[ctl_id].min);
	}
	if (wm->ctl[ctl_id].set)
		wm->ctl[ctl_id].set(wm, regval1, regval2);
	else {
		val = wm->regs[wm->ctl[ctl_id].reg1] & ~wm->ctl[ctl_id].mask1;
		val |= regval1 << lsb_offset(wm->ctl[ctl_id].mask1);
		/* both stereo controls in one register */
		if (wm->ctl[ctl_id].flags & WM8766_FLAG_STEREO &&
				wm->ctl[ctl_id].reg1 == wm->ctl[ctl_id].reg2) {
			val &= ~wm->ctl[ctl_id].mask2;
			val |= regval2 << lsb_offset(wm->ctl[ctl_id].mask2);
		}
		//if (wm->ctl[ctl_id].flags & WM8766_FLAG_VOL_UPDATE)
		//	val |= WM8766_VOL_UPDATE;
		snd_wm8766_write(wm, wm->ctl[ctl_id].reg1, val);
		/* stereo controls in different registers */
		if (wm->ctl[ctl_id].flags & WM8766_FLAG_STEREO &&
				wm->ctl[ctl_id].reg1 != wm->ctl[ctl_id].reg2) {
			val = wm->regs[wm->ctl[ctl_id].reg2] & ~wm->ctl[ctl_id].mask2;
			val |= regval2 << lsb_offset(wm->ctl[ctl_id].mask2);
			if (wm->ctl[ctl_id].flags & WM8766_FLAG_VOL_UPDATE)
				val |= WM8766_VOL_UPDATE;
			snd_wm8766_write(wm, wm->ctl[ctl_id].reg2, val);
		}
	}

	return 0;
}

int snd_wm8766_build_controls(struct snd_wm8766 *wm)
{
	int i;

	for (i = 0; i < WM8766_CTL_COUNT; i++)
		if (wm->ctl[i].name) {
			if (wm->ctl[i].type == SNDRV_CTL_ELEM_TYPE_BOOLEAN)
				wm->ctl[i].max = 1;
		}

	return 0;
}

#include <math.h>

/*
 * Equations in log base 10:
 * linear-to-db(x) = log(x) * 20
 * db-to-linear(x) = 10^(x / 20) 
*/
u16 linear_vol_to_dB(u16 linear_vol) {
    float dB;
    u16 reg_val;
    if (linear_vol == 0)
        return 0;
    if(linear_vol > 100)
        linear_vol = 100;
    dB = (log10(linear_vol)-2) * 20;
    reg_val = 0xFF + (int)(dB*2);
    info("vol linear %d => dB %f reg_val 0x%x\n", linear_vol, dB, reg_val);

    return reg_val;
}

void snd_wm8766_set_volume(struct snd_wm8766 *wm, int ch, u16 l_vol, u16 r_vol) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_CH1_VOL + ch, linear_vol_to_dB(l_vol), linear_vol_to_dB(r_vol));
}

void snd_wm8766_playback_switch(struct snd_wm8766 *wm, int ch, u16 active) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_CH1_SW + ch, !!active, 0);
}

void snd_wm8766_playback_phase_inv(struct snd_wm8766 *wm, int ch, u16 invert) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_PHASE1_SW + ch, !!invert, 0);
}

void snd_wm8766_playback_de_emph(struct snd_wm8766 *wm, int ch, u16 deemph) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_DEEMPH1_SW + ch, !!deemph, 0);
}

void snd_wm8766_infi_zero_dect(struct snd_wm8766 *wm, int enable) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_IZD_SW, !!enable, 0);
}

void snd_wm8766_zero_cross_dect(struct snd_wm8766 *wm, int enable) {
	snd_wm8766_ctl_put(wm, WM8766_CTL_ZC_SW, !!enable, 0);
}

void snd_wm8766_set_role(struct snd_wm8766 *wm, int as_master) {
	u16 val = wm->regs[WM8766_REG_DACCTRL3];
	if (!!as_master == !!(val & WM8766_DAC3_MASTER)) {
		info("wm8766 already %s!", as_master ? "master" : "slave");
		return;
	}
	val &= ~WM8766_DAC3_MASTER;
	if (as_master)
		val |= WM8766_DAC3_MASTER;
	snd_wm8766_write(wm, WM8766_REG_DACCTRL3, val);
	info("wm8766 set as %s!\n", as_master ? "master" : "slave");
}

int snd_wm8766_master_lrclk_div(struct snd_wm8766 *wm, int div) {
	u16 div_enc = 0xFFFF;
	int div_map[][2] = {
			{128, 0},
			{192, 1},
			{256, 2},
			{384, 3},
			{512, 4},
			{768, 5}
	};
	for (int i=0; i<ARRAY_SIZE(div_map); i++) {
		if (div_map[i][0] == div) {
			div_enc = div_map[i][1];
			break;
		}
	}
	if (div_enc == 0xFFFF) {
		info("wm8766 invalid master lrclk div.\n");
		return -1;
	}
	u16 val = wm->regs[WM8766_REG_DACCTRL3];
	val &= ~WM8766_DAC3_MSTR_LRCLK_MASK;

	val |= div_enc << lsb_offset(WM8766_DAC3_MSTR_LRCLK_MASK);
	snd_wm8766_write(wm, WM8766_REG_DACCTRL3, val);
	return 0;
}

void snd_wm8766_clk_polarity(struct snd_wm8766 *wm, int lrclk_pol_inv, int bclk_pol_inv) {
	u16 val = wm->regs[WM8766_REG_IFCTRL];

	val &= ~WM8766_IF_LRC_INVERTED;
	val &= ~WM8766_IF_BCLK_INVERTED;
	if (lrclk_pol_inv)
		val |= WM8766_IF_LRC_INVERTED;
	if (bclk_pol_inv)
		val |= WM8766_IF_BCLK_INVERTED;

	snd_wm8766_write(wm, WM8766_REG_IFCTRL, val);
}
#if 0
void snd_wm8766_test(void) {
    snd_wm8766_init(wm);
    snd_wm8766_set_if(wm, WM8766_IF_FMT_I2S | WM8766_IF_IWL_16BIT);
    snd_wm8766_set_volume(wm, WM8766_CH1, 70, 70);
    snd_wm8766_playback_switch(wm, WM8766_CH1, 1);

}
#endif

/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/widgets/discrete_sliders.h"

namespace Ui {

class CustomWidthSlider final : public SettingsSlider {
public:
	using Ui::SettingsSlider::SettingsSlider;
	void setNaturalWidth(int w) {
		_naturalWidth = w;
	}
	int naturalWidth() const override {
		return _naturalWidth;
	}

private:
	int _naturalWidth = 0;

};

} // namespace Ui

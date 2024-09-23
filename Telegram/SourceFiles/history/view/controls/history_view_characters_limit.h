/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/widgets/labels.h"

namespace HistoryView::Controls {

class CharactersLimitLabel final : public Ui::FlatLabel {
public:
	CharactersLimitLabel(
		not_null<Ui::RpWidget*> parent,
		not_null<Ui::RpWidget*> widgetToAlign,
		style::align align);

	void setLeft(int value);

};

} // namespace HistoryView::Controls

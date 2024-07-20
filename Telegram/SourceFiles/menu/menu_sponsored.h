/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Ui {
class RpWidget;
} // namespace Ui

class HistoryItem;

namespace Menu {

void ShowSponsored(
	not_null<Ui::RpWidget*> parent,
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<HistoryItem*> item);

void ShowSponsoredAbout(std::shared_ptr<ChatHelpers::Show> show);

} // namespace Menu

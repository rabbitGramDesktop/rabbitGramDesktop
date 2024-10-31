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
namespace Menu {
struct MenuCallback;
} // namespace Menu
} // namespace Ui

class HistoryItem;

namespace Menu {

void FillSponsored(
	not_null<Ui::RpWidget*> parent,
	const Ui::Menu::MenuCallback &addAction,
	std::shared_ptr<ChatHelpers::Show> show,
	const FullMsgId &fullId,
	bool mediaViewer,
	bool skipAbout = false);

void ShowSponsored(
	not_null<Ui::RpWidget*> parent,
	std::shared_ptr<ChatHelpers::Show> show,
	const FullMsgId &fullId);

void ShowSponsoredAbout(
	std::shared_ptr<ChatHelpers::Show> show,
	const FullMsgId &fullId);

} // namespace Menu

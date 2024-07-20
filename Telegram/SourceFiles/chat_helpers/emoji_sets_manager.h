/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/layers/box_content.h"

namespace Main {
class Session;
} // namespace Main

namespace Ui {
namespace Emoji {

class ManageSetsBox final : public Ui::BoxContent {
public:
	ManageSetsBox(QWidget*, not_null<Main::Session*> session);

private:
	void prepare() override;

	const not_null<Main::Session*> _session;

};

void LoadAndSwitchTo(not_null<Main::Session*> session, int id);

} // namespace Emoji
} // namespace Ui

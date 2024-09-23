/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Data {
class Birthday;
} // namespace Data

namespace Ui {

class GenericBox;

void EditBirthdayBox(
	not_null<Ui::GenericBox*> box,
	Data::Birthday current,
	Fn<void(Data::Birthday)> save);

} // namespace Ui

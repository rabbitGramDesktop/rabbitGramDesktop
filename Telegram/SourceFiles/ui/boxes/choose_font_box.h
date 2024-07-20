/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Ui {

class GenericBox;

void ChooseFontBox(
	not_null<GenericBox*> box,
	Fn<QImage()> generatePreviewBg,
	const QString &family,
	Fn<void(QString)> save);

} // namespace Ui

/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Ui {
class RpWindow;
} // namespace Ui

namespace Iv {

class Delegate {
public:
	virtual void ivSetLastSourceWindow(not_null<QWidget*> window) = 0;
	[[nodiscard]] virtual QRect ivGeometry() const = 0;
	virtual void ivSaveGeometry(not_null<Ui::RpWindow*> window) = 0;
};

} // namespace Iv

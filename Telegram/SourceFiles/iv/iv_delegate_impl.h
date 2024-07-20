/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "iv/iv_delegate.h"

namespace Iv {

class DelegateImpl final : public Delegate {
public:
	DelegateImpl() = default;

	void ivSetLastSourceWindow(not_null<QWidget*> window) override;
	[[nodiscard]] QRect ivGeometry() const override;
	void ivSaveGeometry(not_null<Ui::RpWindow*> window) override;

private:
	QPointer<QWidget> _lastSourceWindow;

};

} // namespace Iv

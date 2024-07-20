/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "history/view/history_view_sponsored_click_handler.h"

#include "core/click_handler_types.h"

namespace HistoryView {

ClickHandlerPtr SponsoredLink(const QString &link, bool isInternal) {
	class ClickHandler final : public UrlClickHandler {
	public:
		ClickHandler(const QString &link, bool isInternal)
		: UrlClickHandler(link, false)
		, _isInternal(isInternal) {
		}

		QString copyToClipboardContextItemText() const override final {
			return QString();
		}

		QString tooltip() const override final {
			return _isInternal ? QString() : url();
		}

	private:
		const bool _isInternal;

	};

	return std::make_shared<ClickHandler>(link, isInternal);
}

} // namespace HistoryView

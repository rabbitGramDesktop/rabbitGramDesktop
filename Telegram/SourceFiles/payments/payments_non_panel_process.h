/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class HistoryItem;

namespace Window {
class SessionController;
} // namespace Window

namespace Payments {

enum class CheckoutResult;
struct NonPanelPaymentForm;

[[nodiscard]] bool IsCreditsInvoice(not_null<HistoryItem*> item);

Fn<void(NonPanelPaymentForm)> ProcessNonPanelPaymentFormFactory(
	not_null<Window::SessionController*> controller,
	Fn<void(Payments::CheckoutResult)> maybeReturnToBot = nullptr);

Fn<void(NonPanelPaymentForm)> ProcessNonPanelPaymentFormFactory(
	not_null<Window::SessionController*> controller,
	not_null<HistoryItem*> item);

} // namespace Payments

/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "data/data_premium_subscription_option.h"

namespace Api {

[[nodiscard]] Data::PremiumSubscriptionOption CreateSubscriptionOption(
	int months,
	int monthlyAmount,
	int64 amount,
	const QString &currency,
	const QString &botUrl);

template<typename Option>
[[nodiscard]] auto PremiumSubscriptionOptionsFromTL(
		const QVector<Option> &tlOpts) -> Data::PremiumSubscriptionOptions {
	if (tlOpts.isEmpty()) {
		return {};
	}
	auto result = Data::PremiumSubscriptionOptions();
	const auto monthlyAmount = [&] {
		const auto &min = ranges::min_element(
			tlOpts,
			ranges::less(),
			[](const Option &o) { return o.data().vamount().v; }
		)->data();
		return min.vamount().v / float64(min.vmonths().v);
	}();
	result.reserve(tlOpts.size());
	for (const auto &tlOption : tlOpts) {
		const auto &option = tlOption.data();
		auto botUrl = QString();
		if constexpr (!std::is_same_v<Option, MTPPremiumGiftCodeOption>) {
			botUrl = qs(option.vbot_url());
		}
		const auto months = option.vmonths().v;
		const auto amount = option.vamount().v;
		const auto currency = qs(option.vcurrency());
		result.push_back(CreateSubscriptionOption(
			months,
			monthlyAmount,
			amount,
			currency,
			botUrl));
	}
	return result;
}

} // namespace Api

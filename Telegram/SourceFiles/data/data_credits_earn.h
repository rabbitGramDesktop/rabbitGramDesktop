/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "data/data_statistics_chart.h"

#include <QtCore/QDateTime>

namespace Data {

using CreditsEarnInt = uint64;

struct CreditsEarnStatistics final {
	explicit operator bool() const {
		return !!usdRate;
	}
	Data::StatisticalGraph revenueGraph;
	CreditsEarnInt currentBalance = 0;
	CreditsEarnInt availableBalance = 0;
	CreditsEarnInt overallRevenue = 0;
	float64 usdRate = 0.;
	bool isWithdrawalEnabled = false;
	QDateTime nextWithdrawalAt;
	QString buyAdsUrl;
};

} // namespace Data

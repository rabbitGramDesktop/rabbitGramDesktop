/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Statistic {

class AbstractChartView;
enum class ChartViewType;

[[nodiscard]] std::unique_ptr<AbstractChartView> CreateChartView(
	ChartViewType type);

} // namespace Statistic

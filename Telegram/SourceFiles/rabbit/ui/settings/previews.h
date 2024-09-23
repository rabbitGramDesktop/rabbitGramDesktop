/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/rp_widget.h"

class RoundnessPreview : public Ui::RpWidget
{
public:
	RoundnessPreview(QWidget* parent);

protected:
	void paintEvent(QPaintEvent* e) override;
};

class StickerSizePreview : public Ui::RpWidget
{
public:
	StickerSizePreview(QWidget* parent);

protected:
	void paintEvent(QPaintEvent* e) override;
};

class StickerShapePicker : public Ui::RpWidget
{
public:
	StickerShapePicker(QWidget* parent);

protected:
	void paintEvent(QPaintEvent* e) override;
	void mousePressEvent(QMouseEvent *e) override;
};
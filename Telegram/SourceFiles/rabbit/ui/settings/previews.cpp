/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "previews.h"

#include "rabbit/settings/rabbit_settings.h"

#include "core/application.h"
#include "styles/style_layers.h"

#include "main/main_domain.h"
#include "styles/style_rabbit_assets.h"
#include "styles/style_chat.h"
#include "ui/painter.h"
#include "window/main_window.h"

RoundnessPreview::RoundnessPreview(QWidget* parent) : RpWidget(parent) {
    auto sectionHeight = st::rndPreviewSize;
    setMinimumSize(st::boxWidth, sectionHeight);
}

void RoundnessPreview::paintEvent(QPaintEvent* e) {
    Painter p(this);
    PainterHighQualityEnabler hq(p);

    auto size = st::rndPreviewSize;
    auto radius = size * (RabbitSettings::JsonSettings::GetInt("userpic_roundness") / 100.);

    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(st::rndPreviewFill));
    p.drawRoundedRect(
        0, 0,
        size, size,
        radius, radius
    );

    p.setBrush(QBrush(st::rndSkeletonFill));
    auto skeletonWidth = st::boxWidth - (3 * st::rndPreviewSize);
    auto skeletonHeight = st::rndPreviewSize / 5;
    p.drawRoundedRect(
        st::rndPreviewSize * 1.33,
        skeletonHeight,
        skeletonWidth / 2,
        skeletonHeight,
        skeletonHeight / 2,
        skeletonHeight / 2
    );

    p.drawRoundedRect(
        st::rndPreviewSize * 1.33,
        skeletonHeight * 3,
        skeletonWidth,
        skeletonHeight,
        skeletonHeight / 2,
        skeletonHeight / 2
    );
}

StickerSizePreview::StickerSizePreview(QWidget* parent) : RpWidget(parent) {
    auto sectionHeight = st::stickerPreviewSize;
    setMinimumSize(st::boxWidth, sectionHeight);
}

void StickerSizePreview::paintEvent(QPaintEvent* e) {
    Painter p(this);
    PainterHighQualityEnabler hq(p);

    auto size = QSize(RabbitSettings::JsonSettings::GetInt("sticker_size"), RabbitSettings::JsonSettings::GetInt("sticker_size") * 0.7);
    auto radius = []() -> qreal {
        switch (RabbitSettings::JsonSettings::GetInt("sticker_shape")) {
        case 1: return st::bubbleRadiusSmall;
        case 2: return st::bubbleRadiusLarge;
        default: return 0;
        }
    };

    p.setPen(Qt::NoPen);
    p.setBrush(st::rndPreviewFill);
    p.drawRoundedRect(
        QRect(QPoint(0, 0), size),
        radius(), radius());

    p.setBrush(QBrush(st::rndSkeletonFill));
    p.drawRoundedRect(
        size.width() + st::stickerPreviewMargin,
        size.height() - st::stickerPreviewTimeHeight,
        st::stickerPreviewTimeWidth,
        st::stickerPreviewTimeHeight,
        st::stickerPreviewTimeHeight / 2.,
        st::stickerPreviewTimeHeight / 2.
    );

    auto multipliers = { 1.7, 1.5, 1., 1.2 };
    auto topPadding = size.height() + st::stickerPreviewMargin;

    for (auto multiplier : multipliers) {
        auto spacefillerMsgSkeletonWidth = (st::boxWidth / 2) * multiplier;

        p.drawRoundedRect(
            st::boxWidth - spacefillerMsgSkeletonWidth, 
            topPadding,
            spacefillerMsgSkeletonWidth,
            st::stickerSpacefillerHeight,
            st::stickerSpacefillerHeight / 2.,
            st::stickerSpacefillerHeight / 2.
        );

        topPadding += st::stickerPreviewMargin + st::stickerSpacefillerHeight;
    }
}
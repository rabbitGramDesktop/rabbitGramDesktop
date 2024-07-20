/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "dialogs/ui/dialogs_video_userpic.h"

#include "rabbit/settings/rabbit_settings.h"

#include "core/file_location.h"
#include "data/data_peer.h"
#include "data/data_photo.h"
#include "data/data_photo_media.h"
#include "data/data_file_origin.h"
#include "data/data_session.h"
#include "dialogs/ui/dialogs_layout.h"
#include "ui/painter.h"
#include "styles/style_dialogs.h"

namespace Dialogs::Ui {

VideoUserpic::VideoUserpic(not_null<PeerData*> peer, Fn<void()> repaint)
: _peer(peer)
, _repaint(std::move(repaint)) {
}

VideoUserpic::~VideoUserpic() = default;

int VideoUserpic::frameIndex() const {
	return -1;
}

void VideoUserpic::paintLeft(
		Painter &p,
		Ui::PeerUserpicView &view,
		int x,
		int y,
		int w,
		int size,
		bool paused) {
	const auto hq = PainterHighQualityEnabler(p);
	_lastSize = size;

	const auto photoId = _peer->userpicPhotoId();
	if (_videoPhotoId != photoId) {
		_videoPhotoId = photoId;
		_video = nullptr;
		_videoPhotoMedia = nullptr;
		const auto photo = _peer->owner().photo(photoId);
		if (photo->isNull()) {
			_peer->updateFullForced();
		} else {
			_videoPhotoMedia = photo->createMediaView();
			_videoPhotoMedia->videoWanted(
				Data::PhotoSize::Small,
				_peer->userpicPhotoOrigin());
		}
	}
	if (!_video) {
		if (!_videoPhotoMedia) {
			const auto photo = _peer->owner().photo(photoId);
			if (!photo->isNull()) {
				_videoPhotoMedia = photo->createMediaView();
				_videoPhotoMedia->videoWanted(
					Data::PhotoSize::Small,
					_peer->userpicPhotoOrigin());
			}
		}
		if (_videoPhotoMedia) {
			auto small = _videoPhotoMedia->videoContent(
				Data::PhotoSize::Small);
			auto bytes = small.isEmpty()
				? _videoPhotoMedia->videoContent(Data::PhotoSize::Large)
				: small;
			if (!bytes.isEmpty()) {
				auto callback = [=](Media::Clip::Notification notification) {
					clipCallback(notification);
				};
				_video = Media::Clip::MakeReader(
					Core::FileLocation(),
					std::move(bytes),
					std::move(callback));
			}
		}
	}
	if (rtl()) {
		x = w - x - size;
	}
	if (_video && _video->ready()) {
		startReady();

		const auto now = paused ? crl::time(0) : crl::now();

		p.save();

		QPainterPath clipPath;
		QImage frame = _video->current(request(size), now);
		auto radius = frame.height() * RabbitSettings::JsonSettings::GetInt("userpic_roundness") / 100.;
		if (_peer->isForum() && !RabbitSettings::JsonSettings::GetBool("general_roundness")) radius *= .5;
		clipPath.addRoundedRect(
			QRect(x, y, frame.width(), frame.height()),
			radius, radius);
		p.setClipPath(clipPath);
		p.drawImage(x, y, frame);
		p.restore();
	} else {
		_peer->paintUserpicLeft(p, view, x, y, w, size);
	}
}

Media::Clip::FrameRequest VideoUserpic::request(int size) const {
	return {
		.frame = { size, size },
		.outer = { size, size },
		.factor = style::DevicePixelRatio(),
		.radius = ImageRoundRadius::None,
	};
}

bool VideoUserpic::startReady(int size) {
	if (!_video->ready() || _video->started()) {
		return false;
	} else if (!_lastSize) {
		_lastSize = size ? size : _video->width();
	}
	_video->start(request(_lastSize));
	_repaint();
	return true;
}

void VideoUserpic::clipCallback(Media::Clip::Notification notification) {
	using namespace Media::Clip;

	switch (notification) {
	case Notification::Reinit: {
		if (_video->state() == State::Error) {
			_video.setBad();
		} else if (startReady()) {
			_repaint();
		}
	} break;

	case Notification::Repaint: _repaint(); break;
	}
}

void PaintUserpic(
		Painter &p,
		not_null<Entry*> entry,
		PeerData *peer,
		VideoUserpic *videoUserpic,
		PeerUserpicView &view,
		const Ui::PaintContext &context) {
	if (peer) {
		PaintUserpic(
			p,
			peer,
			videoUserpic,
			view,
			context.st->padding.left(),
			context.st->padding.top(),
			context.width,
			context.st->photoSize,
			context.paused);
	} else {
		entry->paintUserpic(p, view, context);
	}
}

void PaintUserpic(
		Painter &p,
		not_null<PeerData*> peer,
		Ui::VideoUserpic *videoUserpic,
		Ui::PeerUserpicView &view,
		int x,
		int y,
		int outerWidth,
		int size,
		bool paused) {
	if (videoUserpic) {
		videoUserpic->paintLeft(p, view, x, y, outerWidth, size, paused);
	} else {
		peer->paintUserpicLeft(p, view, x, y, outerWidth, size);
	}
}

} // namespace Dialogs::Ui

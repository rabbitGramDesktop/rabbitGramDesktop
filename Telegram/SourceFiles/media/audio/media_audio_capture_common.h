/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Media::Capture {

struct Result {
	QByteArray bytes;
	VoiceWaveform waveform;
	crl::time duration;
	bool video = false;
};

} // namespace Media::Capture

/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/chat/attach/attach_abstract_single_preview.h"
#include "ui/chat/attach/attach_controls.h"
#include "base/object_ptr.h"

namespace style {
struct ComposeControls;
} // namespace style

namespace Ui {

class IconButton;

class AbstractSingleFilePreview : public AbstractSinglePreview {
public:
	AbstractSingleFilePreview(
		QWidget *parent,
		const style::ComposeControls &st,
		AttachControls::Type type);
	~AbstractSingleFilePreview();

	[[nodiscard]] rpl::producer<> deleteRequests() const override;
	[[nodiscard]] rpl::producer<> editRequests() const override;
	[[nodiscard]] rpl::producer<> modifyRequests() const override;

protected:
	struct Data {
		QPixmap fileThumb;
		QString name;
		QString statusText;
		int nameWidth = 0;
		int statusWidth = 0;
		bool fileIsAudio = false;
		bool fileIsImage = false;
	};

	void prepareThumbFor(Data &data, const QImage &preview);
	bool isThumbedLayout(Data &data) const;

	void setData(const Data &data);

private:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

	void updateTextWidthFor(Data &data);

	const style::ComposeControls &_st;
	const AttachControls::Type _type;

	Data _data;

	object_ptr<IconButton> _editMedia = { nullptr };
	object_ptr<IconButton> _deleteMedia = { nullptr };

};

} // namespace Ui

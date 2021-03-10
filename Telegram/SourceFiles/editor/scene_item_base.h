/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/unique_qptr.h"

#include <QGraphicsItem>

class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;

namespace Ui {
class PopupMenu;
} // namespace Ui

namespace Editor {

class NumberedItem : public QGraphicsItem {
public:
	using QGraphicsItem::QGraphicsItem;

	void setNumber(int number);
	[[nodiscard]] int number() const;
private:
	int _number = 0;
};

class ItemBase : public NumberedItem {
public:
	enum { Type = UserType + 1 };

	ItemBase(
		rpl::producer<float64> zoomValue,
		std::shared_ptr<float64> zPtr,
		int size,
		int x,
		int y);
	QRectF boundingRect() const override;
	void paint(
		QPainter *p,
		const QStyleOptionGraphicsItem *option,
		QWidget *widget) override;
	int type() const override;
protected:
	enum HandleType {
		None,
		Left,
		Right,
	};
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	QRectF contentRect() const;
	QRectF innerRect() const;
	float64 size() const;
	float64 horizontalSize() const;
	float64 verticalSize() const;
	void setAspectRatio(float64 aspectRatio);

private:
	HandleType handleType(const QPointF &pos) const;
	QRectF rightHandleRect() const;
	QRectF leftHandleRect() const;
	bool isHandling() const;
	void updateVerticalSize();

	const std::shared_ptr<float64> _lastZ;
	const QPen _selectPen;
	const QPen _selectPenInactive;
	const QPen _handlePen;

	base::unique_qptr<Ui::PopupMenu> _menu;

	float64 _scaledHandleSize = 1.0;
	QMarginsF _scaledInnerMargins;

	float64 _horizontalSize = 0;
	float64 _verticalSize = 0;
	float64 _aspectRatio = 1.0;
	HandleType _handle = HandleType::None;

	rpl::lifetime _lifetime;

};

} // namespace Editor
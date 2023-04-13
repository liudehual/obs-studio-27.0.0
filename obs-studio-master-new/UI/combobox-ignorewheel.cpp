#include "combobox-ignorewheel.hpp"

ComboBoxIgnoreScroll::ComboBoxIgnoreScroll(QWidget *parent) : QComboBox(parent)
{
	setFocusPolicy(Qt::StrongFocus);
}
int ComboBoxIgnoreScroll::test()
{
	return 0;
}
void ComboBoxIgnoreScroll::wheelEvent(QWheelEvent *event)
{
	if (!hasFocus())
		event->ignore();
	else
		QComboBox::wheelEvent(event);
}

#pragma once

/* This file exists to prevent ->deleteLater from being called on custom-made
 * item widgets in widgets such as list widgets.  We do this to prevent things
 * such as references to sources/etc from getting stuck in the Qt event queue
 * with no way of controlling when they'll be released. */

class QListWidget;
class QListWidgetItem;

QListWidgetItem *TakeListItem(QListWidget *widget, int row);
void DeleteListItem(QListWidget *widget, QListWidgetItem *item);
void ClearListItems(QListWidget *widget);

template<typename QObjectPtr>
void InsertQObjectByName(std::vector<QObjectPtr> &controls, QObjectPtr control)
{
	QString name = control->objectName();
	auto finder = [name](QObjectPtr elem) {
		return name.localeAwareCompare(elem->objectName()) < 0;
	};
	auto found_at = std::find_if(controls.begin(), controls.end(), finder);

	controls.insert(found_at, control);
}

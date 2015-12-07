//
// Copyright (C) 2015 University of Amsterdam
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef BACKSTAGEOSF_H
#define BACKSTAGEOSF_H

#include <QLineEdit>

#include "backstagepage.h"

#include "fsbmosf.h"
#include "fsbrowser.h"
#include "breadcrumbs.h"

#include <QLabel>
#include <QCursor>

class BackstageOSF : public BackstagePage
{
	Q_OBJECT
public:
	explicit BackstageOSF(QWidget *parent = 0);

	void setOnlineDataManager(OnlineDataManager *odm);

	void setMode(FileEvent::FileMode mode) OVERRIDE;

signals:
	void dataSetOpened(QString path);

private slots:
	void notifyDataSetSelected(QString path);
	void notifyDataSetOpened(QString path);

	void openFile(const QString &nodePath, const QString &filename);
	void userDetailsReceived();

private:

	void updateUserDetails();

	OnlineDataManager *_odm;

	BreadCrumbs *_breadCrumbs;
	FSBMOSF *_model;
	FSBrowser *_fsBrowser;
	QLabel *_nameLabel;
	QWidget *_fileNameContainer;
	QLineEdit *_fileNameTextBox;
	QPushButton *_saveButton;

	class HyperlinkLabel : public QLabel
	{
	public:
		HyperlinkLabel(QWidget *parent)
			: QLabel(parent)
		{
			setOpenExternalLinks(true);
			setCursor(Qt::PointingHandCursor);
			setTextFormat(Qt::RichText);
			setStyleSheet("color : blue ; text-decoration: underline ;");
		}
	};

};

#endif // BACKSTAGEOSF_H

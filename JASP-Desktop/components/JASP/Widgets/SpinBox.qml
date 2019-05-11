//
// Copyright (C) 2013-2018 University of Amsterdam
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

import QtQuick 2.11
import QtQuick.Controls 2.5
import JASP.Widgets 1.0
import JASP.Theme 1.0
import JASP 1.0

FocusScope
{
					id:						root
	property alias	value:					valueField.text
	property double defaultValue:			0
	property alias	doubleValidator:		doubleValidator
	property bool	negativeValues:			false
	property double	min:					negativeValues ? -Infinity : 0
	property double	max:					Infinity
	property int	decimals:				0
	property string inclusive:				"yes"
	property alias	to:						root.max
	property alias	from:					root.min
	property double	lastValidValue:			defaultValue
	property double	stepSize:				1

					width:					minus.width + valueField.width + plus.width
					height:					valueField.height

					Keys.onLeftPressed:		{ minus.clicked(); event.accepted = true; }
					Keys.onRightPressed:	{ plus.clicked();  event.accepted = true; }
					Keys.onEnterPressed:	valueField.focus = !valueField.focus;
					Keys.onReturnPressed:	valueField.focus = !valueField.focus;
					Keys.onEscapePressed:	focus = false;

	function setValue(val)
	{
		var pow			= Math.pow(10, decimals);
		val				= Math.round(val * pow) / pow;
		val				= Math.min(root.max, Math.max(root.min, val));
		valueField.text = String(val);
	}

	RectangularButton
	{
		id:				minus
		iconSource:		"qrc:/icons/subtraction-sign-small.svg" //"qrc:/icons/addition-sign-small.svg"
		onClicked:		root.setValue(Number(valueField.text) - root.stepSize)
		width:			height
		anchors.right:	valueField.left

	}

	TextField
	{
		id:						valueField
		validator:				JASPDoubleValidator { id: doubleValidator; bottom: root.min; top: root.max ; decimals: root.decimals; inclusive: root.inclusive }
		anchors.centerIn:		parent
		width:					Theme.spinBoxWidth
		height:					plus.height
		font:					Theme.font
		horizontalAlignment:	Text.AlignHCenter

		Keys.onReturnPressed:	valueField.processInput()
		Keys.onEnterPressed:	valueField.processInput()
		Keys.onEscapePressed:	{ text = root.lastValidValue; focus = false; }
		onTextChanged:			if(acceptableInput) root.lastValidValue = text

		function processInput()
		{
			if (!acceptableInput)	text				= root.lastValidValue;
			else					root.lastValidValue = Number(text)
			focus = false;
		}
		onActiveFocusChanged:	if(!activeFocus) focus = false;
	}

	RectangularButton
	{
		id:				plus
		iconSource:		"qrc:/icons/addition-sign-small.svg"
		onClicked:		root.setValue(Number(valueField.text) + root.stepSize)
		width:			height
		anchors.left:	valueField.right
	}

	Rectangle
	{
		anchors.fill:		parent
		anchors.margins:	-border.width
		z:					-1
		border.color:		Theme.focusBorderColor
		border.width:		Theme.jaspControlHighlightWidth
		color:				"transparent"
		visible:			root.activeFocus
	}
}

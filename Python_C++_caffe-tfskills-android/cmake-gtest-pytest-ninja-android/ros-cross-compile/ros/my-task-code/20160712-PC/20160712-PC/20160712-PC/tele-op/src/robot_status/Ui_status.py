# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/haijunz/sync-git/tele-op/src/status/status.ui'
#
# Created: Wed Jul 13 11:31:06 2016
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(1025, 300)
        self.monitorlabel = QtGui.QLabel(Form)
        self.monitorlabel.setGeometry(QtCore.QRect(30, 110, 66, 17))
        self.monitorlabel.setText(_fromUtf8(""))
        self.monitorlabel.setObjectName(_fromUtf8("monitorlabel"))
        self.localizationlabel = QtGui.QLabel(Form)
        self.localizationlabel.setGeometry(QtCore.QRect(10, 140, 1001, 17))
        self.localizationlabel.setObjectName(_fromUtf8("localizationlabel"))
        self.voxmaplabel = QtGui.QLabel(Form)
        self.voxmaplabel.setGeometry(QtCore.QRect(10, 170, 1001, 17))
        self.voxmaplabel.setObjectName(_fromUtf8("voxmaplabel"))
        self.cpulabel = QtGui.QLabel(Form)
        self.cpulabel.setGeometry(QtCore.QRect(10, 40, 1001, 21))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.cpulabel.sizePolicy().hasHeightForWidth())
        self.cpulabel.setSizePolicy(sizePolicy)
        self.cpulabel.setObjectName(_fromUtf8("cpulabel"))
        self.memlabel = QtGui.QLabel(Form)
        self.memlabel.setGeometry(QtCore.QRect(10, 70, 1001, 20))
        self.memlabel.setObjectName(_fromUtf8("memlabel"))
        self.disklabel = QtGui.QLabel(Form)
        self.disklabel.setGeometry(QtCore.QRect(10, 100, 1001, 21))
        self.disklabel.setObjectName(_fromUtf8("disklabel"))
        self.exitbtn = QtGui.QPushButton(Form)
        self.exitbtn.setGeometry(QtCore.QRect(10, 240, 98, 27))
        self.exitbtn.setObjectName(_fromUtf8("exitbtn"))
        self.movebaselabel = QtGui.QLabel(Form)
        self.movebaselabel.setGeometry(QtCore.QRect(10, 200, 1001, 17))
        self.movebaselabel.setObjectName(_fromUtf8("movebaselabel"))
        self.restartbtn = QtGui.QPushButton(Form)
        self.restartbtn.setGeometry(QtCore.QRect(160, 240, 98, 27))
        self.restartbtn.setObjectName(_fromUtf8("restartbtn"))

        self.retranslateUi(Form)
        QtCore.QObject.connect(self.exitbtn, QtCore.SIGNAL(_fromUtf8("clicked()")), Form.close)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "robot status", None))
        self.localizationlabel.setText(_translate("Form", "Localization not sync", None))
        self.voxmaplabel.setText(_translate("Form", "voxmap not sync", None))
        self.cpulabel.setText(_translate("Form", "cpu not sync", None))
        self.memlabel.setText(_translate("Form", "mem not sync", None))
        self.disklabel.setText(_translate("Form", "disk not sync", None))
        self.exitbtn.setText(_translate("Form", "Exit", None))
        self.movebaselabel.setText(_translate("Form", "movebase not sync", None))
        self.restartbtn.setText(_translate("Form", "Restart", None))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Form = QtGui.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())


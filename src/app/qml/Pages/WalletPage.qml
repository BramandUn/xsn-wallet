import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import "../Views"
import "../Components"
import "../Popups"

import com.xsn.viewmodels 1.0
import com.xsn.models 1.0

Page {
    id: root
    property int windowWidth: 0
    property int currentAssetID: assetsListView.currentItem ? assetsListView.currentItem.id : 0
    property string currentAssetName: assetsListView.currentItem ? assetsListView.currentItem.name : ""
    property string currentAssetColor: assetsListView.currentItem ? assetsListView.currentItem.color : ""
    property string currentAssetSymbol: assetsListView.currentItem ? assetsListView.currentItem.symbol : ""
    background: Rectangle {
        color: "transparent"
    }

    WalletAssetViewModel {
        id: walletViewModel
        Component.onCompleted: {
            initialize(ApplicationViewModel);
        }
        currentAssetID: root.currentAssetID
    }

    Component{
        id: sendDialogComponent
        SendPopup {

        }
    }

    Component{
        id: receiveDialogComponent
        ReceivePopup {
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: windowWidth > windowWidthSmallMode ? assetsViewWidthLargeMode : assetsViewWidthSmallMode
            color: "#16192E"

            Item {
                anchors.fill: parent
                anchors.topMargin: 1

                WalletAssetsListView {
                    id: assetsListView
                    anchors.fill: parent
                    model: WalletAssetsListModel {
                        Component.onCompleted: initialize(ApplicationViewModel)

                        onModelReset: {
                            assetsListView.currentIndex = 0;
                        }
                    }
                }

            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 30
            spacing: 35

            PageHeaderView {
                Layout.fillHeight: true
                Layout.fillWidth: true

                mainHeader: currentAssetName
                coinSymbol: currentAssetSymbol
                windowWidth: root.windowWidth
                buttonsVisible: true

                onSendCoins: {
                    var dialog = sendDialogComponent.createObject(root)
                    dialog.open();
                }


                onReceiveCoins: {
                    var dialog = receiveDialogComponent.createObject(root)
                    dialog.receivingAddress = walletViewModel.getReceivingAddress();
                    dialog.open();
                }
            }

            TransactionsListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                transactionListModel: walletViewModel.transactionsListModel
            }
        }
    }
}

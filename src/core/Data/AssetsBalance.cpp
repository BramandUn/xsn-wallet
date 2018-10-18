#include "AssetsBalance.hpp"
#include "Data/WalletAssetsModel.hpp"
#include "Models/AllTransactionsDataSource.hpp"
#include "QDebug"

//==============================================================================

AssetsBalance::AssetsBalance(QPointer<WalletAssetsModel> walletAssetsModel, QPointer<AllTransactionsDataSource> dataSource, QObject *parent) :
    QObject(parent),
    _walletAssetsModel(walletAssetsModel),
    _dataSource(dataSource)
{
    init();
}

//==============================================================================

AssetsBalance::~AssetsBalance()
{

}

//==============================================================================

auto AssetsBalance::balance() const -> BalanceMap
{
    return _assetsBalance;
}

//==============================================================================

Balance AssetsBalance::balanceSum() const
{
    if(_assetsBalance.empty())
    {
        Balance result = 0;
        for(auto assetBalance : _assetsBalance)
            result += assetBalance.second;

        return result;
    }
    else
        return 0;
}

//==============================================================================

Balance AssetsBalance::balanceById(AssetID assetID) const
{
    return _assetsBalance.at(assetID);
}

//==============================================================================

void AssetsBalance::onTransactionFetched(WalletDataSource::TransactionsList list)
{
    _transactionList.swap(list);
    auto assetsModel = _walletAssetsModel->assets();

    updateBalance();
}

//==============================================================================

void AssetsBalance::onTransactionAdded(TransactionEntry entry)
{
    _transactionList.emplace_back(entry);
    updateBalance();
}

//==============================================================================

void AssetsBalance::updateBalance()
{
    auto assetsModel = _walletAssetsModel->assets();

    for(auto transaction : _transactionList)
    {
        auto it  = _assetsBalance.find(transaction._assetID);
        if(it == std::end(_assetsBalance))
        {
            _assetsBalance.insert({transaction._assetID, calculateBalance(transaction)});
        }
        else
        {
            qDebug() << "balance" <<  _assetsBalance.at(transaction._assetID) + calculateBalance(transaction);
            it->second = _assetsBalance.at(transaction._assetID) + calculateBalance(transaction);
        }

    }
    balanceUpdated();
}

//==============================================================================

Balance AssetsBalance::calculateBalance(TransactionEntry transaction)
{
    switch (transaction._txType)
    {
    case TransactionEntry::Type::Sent: return -transaction._delta;
    case TransactionEntry::Type::Received: return transaction._delta;
    }
}

//==============================================================================

void AssetsBalance::init()
{
    if(_dataSource)
    {
        connect(_dataSource, &AllTransactionsDataSource::transactionsFetched,
                this, &AssetsBalance::onTransactionFetched);
        connect(_dataSource, &AllTransactionsDataSource::transactionAdded,
                this, &AssetsBalance::onTransactionAdded);
        _dataSource->fetchTransactions();
    }
}

//==============================================================================

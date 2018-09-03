#include "ApplicationViewModel.hpp"
#include "Models/EmulatorWalletDataSource.hpp"

//==============================================================================

ApplicationViewModel::ApplicationViewModel(QObject *parent) : QObject(parent)
{
    init();
}

//==============================================================================

ApplicationViewModel::~ApplicationViewModel()
{

}

//==============================================================================

WalletDataSource* ApplicationViewModel::dataSource() const
{
    return _emulatorWalletDataSource.data();
}

//==============================================================================

ApplicationViewModel *ApplicationViewModel::Instance()
{
    static ApplicationViewModel instance;
    return &instance;
}

//==============================================================================

void ApplicationViewModel::init()
{
    initDataSource();
}

//==============================================================================

void ApplicationViewModel::initDataSource()
{
    _emulatorWalletDataSource = new EmulatorWalletDataSource();
}

//==============================================================================


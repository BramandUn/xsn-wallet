#ifndef TST_KEYSTORAGE_HPP
#define TST_KEYSTORAGE_HPP

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <Storage/KeyStorage.hpp>
#include <walletdb.h>
#include <utilstrencodings.h>

using namespace testing;

class KeyStorageTest : public ::testing::Test
{
protected:

    KeyStorage storage;
    bitcoin::CHDChain hdChain;

    KeyStorageTest()
    {
    }

    virtual void SetUp()
    {
        bitcoin::ECC_Start();
        auto seed = bitcoin::ParseHex("4b381541583be4423346c643850da4b320e46a87ae3d2a4e6da11eba819cd4acba45d239319ac14f863b8d5ab5a0d0c64d2e8a1e7d1457df2e5a3c51c73235be");
        hdChain.vchSeed = SecureVector(seed.begin(), seed.end());
        storage.setHDChain(hdChain);
    }

    virtual void TearDown()
    {
        bitcoin::ECC_Stop();
    }
};

TEST_F(KeyStorageTest, ReplacingHDChain)
{
    ExtendedKeyPathBip44 keyPath(0, 0);
    keyPath.addChild(0);

    auto firstExtKey = storage.deriveKeyForPath(keyPath);
    auto backupChain = storage.hdChain();
    storage.setHDChain(bitcoin::CHDChain()); // set default chain with zero seed
    ASSERT_NE(firstExtKey, storage.deriveKeyForPath(keyPath));
    storage.setHDChain(backupChain);
    ASSERT_EQ(firstExtKey, storage.deriveKeyForPath(keyPath));
}

TEST_F(KeyStorageTest, PrivateKeyPathDerivationChange)
{
    for(unsigned coinType = 0; coinType < 10; ++coinType)
    {
        for(unsigned accountIndex = 0; accountIndex < 10; ++accountIndex)
        {
            const unsigned keysCount = 10;

            for(unsigned i = 0; i < keysCount; ++i)
            {
                ExtendedKeyPathBip44 expectedValue(coinType, accountIndex, true);
                expectedValue.addChild(i);
                auto actualValue = storage.deriveNewChildKey(coinType, accountIndex, true).first;
                ASSERT_EQ(expectedValue, actualValue) << "BIP44 path doesn't match expected: " << expectedValue.toString().toStdString() << " actual: " << actualValue.toString().toStdString();
                ASSERT_EQ(storage.hdChain().accounts.at(coinType).at(accountIndex).nInternalChainCounter, i + 1);
            }
        }
    }
}

TEST_F(KeyStorageTest, PrivateKeyPathDerivation)
{
    for(unsigned coinType = 0; coinType < 10; ++coinType)
    {
        for(unsigned accountIndex = 0; accountIndex < 10; ++accountIndex)
        {
            const unsigned keysCount = 10;

            for(unsigned i = 0; i < keysCount; ++i)
            {
                ExtendedKeyPathBip44 expectedValue(coinType, accountIndex, false);
                expectedValue.addChild(i);
                auto actualValue = storage.deriveNewChildKey(coinType, accountIndex, false).first;
                ASSERT_EQ(expectedValue, actualValue) << "BIP44 path doesn't match expected: " << expectedValue.toString().toStdString() << " actual: " << actualValue.toString().toStdString();
                ASSERT_EQ(storage.hdChain().accounts.at(coinType).at(accountIndex).nExternalChainCounter, i + 1);
            }
        }
    }
}


#endif // TST_KEYSTORAGE_HPP

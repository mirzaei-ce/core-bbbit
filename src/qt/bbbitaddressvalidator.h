// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BBBIT_QT_BBBITADDRESSVALIDATOR_H
#define BBBIT_QT_BBBITADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BbbitAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BbbitAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Bbbit address widget validator, checks for a valid bbbit address.
 */
class BbbitAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BbbitAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BBBIT_QT_BBBITADDRESSVALIDATOR_H

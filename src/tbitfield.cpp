// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0) {
        throw -1;
    }
    BitLen = len;
    MemLen = len / sizeof(TELEM) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int result;
    if (n < 0 || n > (BitLen - 1)) {
        throw -1;
    }
    result = n / 32;
    return result;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > (BitLen - 1)) {
        throw -1;
    }
    TELEM mask = 1;
    mask = mask << (n % 32);
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >(BitLen - 1)) {
        throw -1;
    }
    int ind = GetMemIndex(n); //нужная ячейка
    TELEM mask = GetMemMask(n);
    pMem[ind] = pMem[ind] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > (BitLen - 1)) {
        throw - 1;
    }
    int ind = GetMemIndex(n);
    TELEM mask = ~GetMemMask(n);
    pMem[ind] = pMem[ind] & mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > (BitLen - 1)) {
        throw -1;
    }
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return pMem[ind] & mask;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) {
        return *this;
    }
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        pMem = new TELEM[bf.MemLen];
        MemLen = bf.MemLen;
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < BitLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < (MemLen - 1); i++) {
        if (this->GetBit(i) != bf.GetBit(i)) {
            return 0;
        }
    }
    return 1; //Сделать через ->>>> (MemLen - 1)*sizeof(TELEM) * 32;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = bf.BitLen;
    int minLen = this->BitLen;
    if (this->BitLen > bf.BitLen) {
        maxLen = this->BitLen;
        minLen = bf.BitLen;
    }

    TBitField result(maxLen);
    for (int i = 0; i < minLen; i++) {
        result.pMem[i] = this->pMem[i] | bf.pMem[i];
    }

    if (this->BitLen > bf.BitLen) {
        for (int i = minLen; i < this->MemLen; i++) {
            result.pMem[i] = this->pMem[i]; 
        }
    }
    else {
        for (int i = minLen; i < bf.MemLen; i++) {
            result.pMem[i] = bf.pMem[i]; 
        }
    }
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = bf.BitLen;
    int minLen = this->BitLen;
    if (this->BitLen > bf.BitLen) {
        maxLen = this->BitLen;
        minLen = bf.BitLen;
    }
    TBitField result(maxLen);
    int i = 0;
    for (i = 0; i < minLen; i++) {
        result.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(this->MemLen);
    for (int i = 0; i < this->MemLen; i++) {
        result.pMem[i] = ~pMem[i]; //Доработать (Мусор в хвосте)
    }
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int bit;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> bit;
        if (bit) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i) == 0) {
            ostr << 0;
        }
        else {
            ostr << 1;
        }
    }
    return ostr;
}

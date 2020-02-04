#include <QtTest/QtTest>

#include "PageLabelInfo_p.h"

class TestPageLabelInfo : public QObject
{
    Q_OBJECT
private slots:
    void testToRoman();
    void testFromRoman();
    void testToLatin();
    void testFromLatin();
};

void TestPageLabelInfo::testToRoman()
{
    GooString str;
    toRoman(177, &str, false);
    QCOMPARE (str.c_str(), "clxxvii");
}

void TestPageLabelInfo::testFromRoman()
{
    GooString roman("clxxvii");
    QCOMPARE(fromRoman(roman.c_str()), 177);
}

void TestPageLabelInfo::testToLatin()
{
    GooString str;
    toLatin(54, &str, false);
    QCOMPARE(str.c_str(), "bbb");
}

void TestPageLabelInfo::testFromLatin()
{
    GooString latin("ddd");
    QCOMPARE(fromLatin(latin.c_str()), 56);
}

QTEST_MAIN(TestPageLabelInfo)
#include "check_pagelabelinfo.moc"


/****************************************************************************
**
** Modified by demir.ai
** https://ahmetfurkandemir.com/en/
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // birinci derecedeki onemli anahtar kelimeler.
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bint\\b" << "\\bdouble\\b"
                    << "\\bstring\\b" << "\\bfloat\\b" << "\\blong\\b"
                    << "\\bshort\\b" << "\\bvoid\\b" << "\\bextern\\b"
                    << "\\bclasss\\b" << "\\bstruct\\b" << "\\btypedef\\b"
                    << "\\btemplate\\b" << "\\bunion\\b" << "\\bunsigned\\b"
                    << "\\bbool\\b"

                    << "\\breturn\\b" << "\\belse\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bnamespace\\b"
                    << "\\bprivate:\\b" << "\\bpublic:\\b" << "\\bstatic\\b"
                    << "\\btypename\\b" << "\\typename\\b" << "\\bvirtual\\b"
                    << "\\bvolatile\\b" << "\\bdo\\b" << "\\bbreak\\b"
                    << "\\bcontinue\\b" << "\\bcase\\b" << "\\bdefault\\b"
                    << "\\busing\\b"

                    << "\\btrue\\b" << "\\bfalse\\b";


    // ikinci derecedeki onemli anahtar kelimeler.

    methodFormat.setForeground(QColor(103,216,239,255));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = methodFormat;
    highlightingRules.append(rule);

    functionFormat.setForeground(QColor(166,226,44,255));
    rule.pattern = QRegularExpression("\\.* \\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);


    includeClassFormat.setForeground(QColor(231,219,116,255));
    rule.pattern = QRegularExpression("#include.*<.*>");
    rule.format = includeClassFormat;
    highlightingRules.append(rule);

    defineFormat.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("#define");
    rule.format = defineFormat;
    highlightingRules.append(rule);

    ifFormat0.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("if+(?=\\()");
    rule.format = ifFormat0;
    highlightingRules.append(rule);

    ifFormat1.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("if.* +(?=\\()");
    rule.format = ifFormat1;
    highlightingRules.append(rule);

    forFormat0.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("for+(?=\\()");
    rule.format = forFormat0;
    highlightingRules.append(rule);

    forFormat1.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("for.* +(?=\\()");
    rule.format = forFormat1;
    highlightingRules.append(rule);

    whileFormat0.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("while+(?=\\()");
    rule.format = whileFormat0;
    highlightingRules.append(rule);

    whileFormat1.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("while.* +(?=\\()");
    rule.format = whileFormat1;
    highlightingRules.append(rule);

    includeFormat.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("#include");
    rule.format = includeFormat;
    highlightingRules.append(rule);

    numberFormat1.setForeground(QColor(172,128,255,255));
    rule.pattern = QRegularExpression("[0-9]");
    rule.format = numberFormat1;
    highlightingRules.append(rule);

    numberFormat0.setForeground(Qt::white);
    rule.pattern = QRegularExpression("[A-Za-z][0-9]");
    rule.format = numberFormat0;
    highlightingRules.append(rule);

    SwitchFormat0.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("switch+(?=\\()");
    rule.format = SwitchFormat0;
    highlightingRules.append(rule);

    SwitchFormat1.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("switch.* +(?=\\()");
    rule.format = SwitchFormat1;
    highlightingRules.append(rule);

    SizeofFormat0.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("sizeof+(?=\\()");
    rule.format = SizeofFormat0;
    highlightingRules.append(rule);

    SizeofFormat1.setForeground(QColor(249,36,114,255));
    rule.pattern = QRegularExpression("sizeof.* +(?=\\()");
    rule.format = SizeofFormat1;
    highlightingRules.append(rule);


    // birinci derecedeki onemli anahtar kelimeleri renklendirme.
    int count = -1;
    foreach (QString pattern, keywordPatterns) {

        count++;

        if(count<=15){

            keywordFormat.setForeground(QColor(103,216,239,255));
            keywordFormat.setFontItalic(true);

            rule.pattern = QRegularExpression(pattern);

            rule.format = keywordFormat;
            highlightingRules.append(rule);

        }

        else if(count>15 && count<=34){

            keywordFormat.setForeground(QColor(249,36,114,255));
            keywordFormat.setFontItalic(false);
            rule.pattern = QRegularExpression(pattern);

            rule.format = keywordFormat;
            highlightingRules.append(rule);

        }

        else{

            keywordFormat.setForeground(QColor(172,128,255,255));
            keywordFormat.setFontItalic(false);
            rule.pattern = QRegularExpression(pattern);

            rule.format = keywordFormat;
            highlightingRules.append(rule);

        }

    }

    // ucuncu derecedeki onemli anahtar kelimeler.
    quotationFormat.setForeground(QColor(231,219,116,255));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotes.setForeground(QColor(231,219,116,255));
    rule.pattern = QRegularExpression("'.*'");
    rule.format = quotes;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor(83,78,74));
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(83,78,74));

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");


}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
//! [7] //! [8]
    setCurrentBlockState(0);
//! [8]

//! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

//! [9] //! [10]
    while (startIndex >= 0) {
//! [10] //! [11]
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
//! [11]

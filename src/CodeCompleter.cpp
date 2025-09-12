#include "CodeCompleter.h"

CodeCompleter::CodeCompleter(QObject *parent) : QObject(parent) {
    index = clang_createIndex(0, 0);
}

QStringList CodeCompleter::completeAt(const QString &file, int line, int column) {
    QStringList results;

    CXTranslationUnit unit = clang_parseTranslationUnit(
        index, file.toUtf8().data(), nullptr, 0,
        nullptr, 0, CXTranslationUnit_None);

    if (!unit) return results;

    CXCodeCompleteResults *ccr = clang_codeCompleteAt(
        unit, file.toUtf8().data(), line, column, nullptr, 0,
        clang_defaultCodeCompleteOptions());

    if (!ccr) return results;

    for (unsigned i = 0; i < ccr->NumResults; i++) {
        CXCompletionResult res = ccr->Results[i];
        CXCompletionString str = res.CompletionString;

        QString completion;
        for (unsigned j = 0; j < clang_getNumCompletionChunks(str); j++) {
            if (clang_getCompletionChunkKind(str, j) == CXCompletionChunk_TypedText) {
                CXString txt = clang_getCompletionChunkText(str, j);
                completion = QString::fromUtf8(clang_getCString(txt));
                clang_disposeString(txt);
            }
        }
        if (!completion.isEmpty()) results << completion;
    }

    clang_disposeCodeCompleteResults(ccr);
    clang_disposeTranslationUnit(unit);
    return results;
}
CodeCompleter::~CodeCompleter() {
    clang_disposeIndex(index);
}
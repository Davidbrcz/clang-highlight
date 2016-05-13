//===--- AnnotatedToken.h - clang-highlight ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_CLANG_HIGHLIGHT_ANNOTATED_TOKEN_H
#define LLVM_CLANG_TOOLS_CLANG_HIGHLIGHT_ANNOTATED_TOKEN_H

#include "clang/Lex/Lexer.h"
#include <memory>
#include <cstddef>

namespace clang {
namespace fuzzy {

class ASTElement;

class AnnotatedToken {
  Token Tok_;
  ASTElement *Annot;

public:
  AnnotatedToken(Token Tok) : Tok_(Tok), Annot(nullptr) {}

  StringRef getText(const SourceManager &SourceMgr) const {
    return StringRef(SourceMgr.getCharacterData(Tok().getLocation()),
                     Tok().getLength());
  }

  tok::TokenKind getTokenKind() const { return Tok().getKind(); }

  Token& Tok() { return Tok_; }
  const Token& Tok() const { return Tok_; }

  void setASTReference(ASTElement *ASTReference) { Annot = ASTReference; }
  const ASTElement *getASTReference() const { return Annot; }
  ASTElement *getASTReference() { return Annot; }
  bool hasASTReference() const { return Annot; }
};

class AnnotatedTokenRef {
  AnnotatedToken *ATok;

public:
  AnnotatedTokenRef(AnnotatedToken *ATok, ASTElement *AstRef) : ATok(ATok) {
    if (ATok)
      ATok->setASTReference(AstRef);
  }
  AnnotatedTokenRef(std::nullptr_t  = nullptr) : ATok(nullptr) {}
  AnnotatedTokenRef(const AnnotatedTokenRef &) = default;
  AnnotatedTokenRef(AnnotatedTokenRef &&) = default;

  AnnotatedTokenRef(AnnotatedTokenRef const &o, ASTElement *AstRef)
      : ATok(o.ATok) {
    if (ATok)
      ATok->setASTReference(AstRef);
  }

  AnnotatedTokenRef &operator=(const AnnotatedTokenRef &) = default;
  AnnotatedTokenRef &operator=(AnnotatedTokenRef &&) = default;

  operator bool() const { return ATok; }
  AnnotatedToken *get() { return ATok; }
  AnnotatedToken *get() const { return ATok; }

  AnnotatedToken &getRef() {
    assert(*this);
    return *ATok;
  }
  const AnnotatedToken &getRef() const {
    assert(*this);
    return *ATok;
  }

  AnnotatedToken &operator*() { return getRef(); }
  const AnnotatedToken &operator*() const { return getRef(); }
  AnnotatedToken *operator->() { return &getRef(); }
  const AnnotatedToken *operator->() const { return &getRef(); }

  AnnotatedTokenRef &operator=(AnnotatedToken *ATok) {
    this->ATok = ATok;
    return *this;
  }
};

} // end namespace fuzzy
} // end namespace clang

#endif // LLVM_CLANG_TOOLS_CLANG_HIGHLIGHT_ANNOTATED_TOKEN_H

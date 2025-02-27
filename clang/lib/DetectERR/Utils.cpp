//
// Created by machiry on 10/16/21.
//

#include "clang/DetectERR/Utils.h"
#include "clang/DetectERR/PersistentSourceLoc.h"
#include "clang/AST/Expr.h"

using namespace clang;

FuncId getFuncID(const clang::FunctionDecl *FD, ASTContext *C) {
  FuncId RetFID;
  auto PSL = PersistentSourceLoc::mkPSL(FD, *C);
  RetFID.first = FD->getNameAsString();
  RetFID.second = PSL.getFileName();
  return RetFID;
}

const Expr *removeAuxillaryCasts(const Expr *E) {
  bool NeedStrip = true;
  while (NeedStrip) {
    NeedStrip = false;
    E = E->IgnoreParenImpCasts();
    if (const CStyleCastExpr *C = dyn_cast<CStyleCastExpr>(E)) {
      E = C->getSubExpr();
      NeedStrip = true;
    }
  }
  return E;
}

bool isNULLExpr(const clang::Expr *E, ASTContext &C) {
  QualType Typ = E->getType();
  E = removeAuxillaryCasts(E);
  return Typ->isPointerType() && E->isIntegerConstantExpr(C) &&
         E->isNullPointerConstant(C, Expr::NPC_ValueDependentIsNotNull);
}

bool isNegativeNumber(const clang::Expr *E, ASTContext &C) {
  E = removeAuxillaryCasts(E);
  if (E->isIntegerConstantExpr(C)) {
    auto NewAPI = E->getIntegerConstantExpr(C);
    if (NewAPI.hasValue()) {
      return (NewAPI->getSExtValue() < 0);
    }
  }
  return false;
}
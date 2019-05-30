#include "Renderator.h"

bool SortPerZOrder(Entitat* a, Entitat* b)
{
	return (a->getZOrder() < b->getZOrder());
}

Renderator* Renderator::Instance()
{
  static Renderator instance;

  return &instance;
}

#include "PowerWorker.h"
#include "PowerProvider.h"

void PowerWorker::work() {
    PowerProvider::updateData();
}

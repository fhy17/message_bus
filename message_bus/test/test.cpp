#include <gtest/gtest.h>
#include "export_interface.h"
#include "dll_proxy.h"
#include "test.hpp"

TEST(exportTestCase, testAdd) {
    auto file = getTestDependencyPath() + "\\export.dll";
    DllProxyPtr dll_proxy = std::make_shared<DllProxy>(file);

    using create_obj_type = ExportInterface*();
    using destroy_obj_type = void(ExportInterface*);
    auto obj = dll_proxy->call<create_obj_type>("createExportObj");
    EXPECT_EQ(obj->add(1, 6), 7);
    dll_proxy->call<destroy_obj_type>("destroyExportObj", obj);
}
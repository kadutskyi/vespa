// Copyright 2017 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include <vespa/vespalib/testkit/test_kit.h>
#include "test_features.h"

using vespalib::eval::DoubleValue;
using vespalib::eval::ValueType;

namespace search {
namespace fef {
namespace test {

//-----------------------------------------------------------------------------

struct ImpureValueExecutor : FeatureExecutor {
    double value;
    ImpureValueExecutor(double value_in) : value(value_in) {}
    void execute(uint32_t) override { outputs().set_number(0, value); }
};

bool
ImpureValueBlueprint::setup(const IIndexEnvironment &, const std::vector<vespalib::string> &params)
{
    ASSERT_EQUAL(1u, params.size());
    value = strtod(params[0].c_str(), nullptr);
    describeOutput("out", "the impure value");
    return true;
}

FeatureExecutor &
ImpureValueBlueprint::createExecutor(const IQueryEnvironment &, vespalib::Stash &stash) const
{
    return stash.create<ImpureValueExecutor>(value);
}

//-----------------------------------------------------------------------------

struct DocidExecutor : FeatureExecutor {
    void execute(uint32_t docid) override { outputs().set_number(0, docid); }
};

bool
DocidBlueprint::setup(const IIndexEnvironment &, const std::vector<vespalib::string> &)
{
    describeOutput("out", "the local document id");
    return true;
}

FeatureExecutor &
DocidBlueprint::createExecutor(const IQueryEnvironment &, vespalib::Stash &stash) const
{
    return stash.create<DocidExecutor>();
}

//-----------------------------------------------------------------------------

struct BoxingExecutor : FeatureExecutor {
    DoubleValue value;
    BoxingExecutor() : value(0.0) {}
    bool isPure() override { return true; }
    void execute(uint32_t) override {
        value = DoubleValue(inputs().get_number(0));
        outputs().set_object(0, value);
    }
};

bool
BoxingBlueprint::setup(const IIndexEnvironment &, const std::vector<vespalib::string> &params)
{
    ASSERT_EQUAL(1u, params.size());
    defineInput(params[0]);
    describeOutput("out", "boxed value", FeatureType::object(ValueType::double_type()));
    return true;
}

FeatureExecutor &
BoxingBlueprint::createExecutor(const IQueryEnvironment &, vespalib::Stash &stash) const
{
    return stash.create<BoxingExecutor>();
}

//-----------------------------------------------------------------------------

} // namespace test
} // namespace fef
} // namespace search

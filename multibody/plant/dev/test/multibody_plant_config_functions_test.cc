#include "drake/multibody/plant/dev/multibody_plant_config_functions.h"

#include <gtest/gtest.h>

#include "drake/common/test_utilities/expect_throws_message.h"

namespace anzu {
namespace sim {
namespace internal {
namespace {

using drake::multibody::ContactModel;

GTEST_TEST(MultibodyPlantConfigFunctionsTest, BasicTest) {
  MultibodyPlantConfig config;
  config.time_step = 0.002;
  config.penetration_allowance = 0.003;
  config.stiction_tolerance = 0.004;
  config.contact_model = "hydroelastic";

  drake::systems::DiagramBuilder<double> builder;
  auto result = AddMultibodyPlant(config, &builder);
  EXPECT_EQ(result.plant.time_step(), 0.002);
  EXPECT_EQ(result.plant.get_contact_model(),
            ContactModel::kHydroelasticsOnly);
  // There is no getter for penetration_allowance nor stiction_tolerance, so we
  // can't test them.
}

GTEST_TEST(MultibodyPlantConfigFunctionsTest, ContactModelTest) {
  std::vector<std::pair<const char*, ContactModel>> known_values{
    std::pair("point", ContactModel::kPointContactOnly),
    std::pair("hydroelastic", ContactModel::kHydroelasticsOnly),
    std::pair("hydroelastic_with_fallback",
        ContactModel::kHydroelasticWithFallback),
  };

  for (const auto& [name, value] : known_values) {
    EXPECT_EQ(GetContactModelFromString(name), value);
    EXPECT_EQ(ContactModelToString(value), name);
  }

  DRAKE_EXPECT_THROWS_MESSAGE(GetContactModelFromString("foobar"),
                              ".*Unknown.*foobar.*");
}

}  // namespace
}  // namespace internal
}  // namespace sim
}  // namespace anzu

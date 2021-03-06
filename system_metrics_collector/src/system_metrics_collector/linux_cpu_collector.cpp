// Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rcutils/logging_macros.h"

#include "../../src/system_metrics_collector/constants.hpp"
#include "../../src/system_metrics_collector/linux_cpu_measurement_node.hpp"

/**
* An entry point that starts the linux system CPU metric collector node.
*/

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  const auto cpu_node =
    std::make_shared<system_metrics_collector::LinuxCpuMeasurementNode>("linuxCpuCollector");

  rclcpp::executors::MultiThreadedExecutor ex;
  cpu_node->configure();
  cpu_node->activate();

  ex.add_node(cpu_node->get_node_base_interface());
  ex.spin();

  rclcpp::shutdown();
  cpu_node->deactivate();

  return 0;
}

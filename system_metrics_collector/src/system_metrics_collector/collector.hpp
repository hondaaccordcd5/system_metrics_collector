// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef SYSTEM_METRICS_COLLECTOR__COLLECTOR_HPP_
#define SYSTEM_METRICS_COLLECTOR__COLLECTOR_HPP_

#include <mutex>
#include <string>

#include "../moving_average_statistics/moving_average.hpp"
#include "../moving_average_statistics/types.hpp"

#include "rcpputils/thread_safety_annotations.hpp"

namespace system_metrics_collector
{

/**
 * Simple class in order to collect observed data and generate statistics for the given observations.
 */
class Collector
{
public:
  Collector() = default;
  virtual ~Collector() = default;

  /**
   * Add an observed measurement. This aggregates the measurement and calculates statistics
   * via the moving_average class.
   *
   * @param the measurement observed
   */
  virtual void AcceptData(const double measurement);

  /**
   * Return the statistics for all of the observed data.
   *
   * @return the StatisticData for all the observed measurements
   */
  virtual moving_average_statistics::StatisticData GetStatisticsResults() const;

  /**
   * Clear / reset all current measurements.
   */
  virtual void ClearCurrentMeasurements();

  /**
   * Return true is start has been called, false otherwise.
   *
   * @return the started state of this collector
   */
  bool IsStarted() const;

  /**
   * Return a pretty printed status representation of this class
   *
   * @return a string detailing the current status
   */
  virtual std::string GetStatusString() const;

  // todo @dabonnie uptime (once start has been called)

protected:
  /**
   * Start collecting data. Meant to be called after construction. Note: this locks the recursive mutex class
   * member 'mutex'.
   *
   * @return true if started, false if an error occurred
   */
  virtual bool Start();

  /**
   * Stop collecting data. Meant to be a teardown method (before destruction, but should place the
   * class in a restartable state, i.e., start can be called to be able to resume collection.
   *
   * This calls ClearCurrentMeasurements.
   *
   * @return true if stopped, false if an error occurred
   */
  virtual bool Stop();

private:
  /**
   * Override in order to perform necessary starting steps.
   *
   * @return true if setup was successful, false otherwise.
   */
  virtual bool SetupStart() = 0 RCPPUTILS_TSA_REQUIRES(mutex_);

  /**
   * Override in order to perform necessary teardown.
   *
   * @return true if teardown was successful, false otherwise.
   */
  virtual bool SetupStop() = 0 RCPPUTILS_TSA_REQUIRES(mutex_);

  mutable std::mutex mutex_;

  moving_average_statistics::MovingAverageStatistics collected_data_;

  bool started_{false} RCPPUTILS_TSA_GUARDED_BY(mutex_);
};

}  // namespace system_metrics_collector

#endif  // SYSTEM_METRICS_COLLECTOR__COLLECTOR_HPP_

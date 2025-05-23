#pragma once

#include "interfaces/sensor.hpp"
#include "utils/conv_container.hpp"
#include "utils/generate_unique_mock_id.hpp"

#include <gmock/gmock.h>

class SensorMock : public interfaces::Sensor
{
  public:
    explicit SensorMock()
    {
        initialize();
    }

    explicit SensorMock(Id sensorId) : mockSensorId(sensorId)
    {
        initialize();
    }

    static Id makeId(std::string_view service, std::string_view path)
    {
        return Id("SensorMock", service, path);
    }

    static std::vector<std::shared_ptr<interfaces::Sensor>> makeSensorMocks(
        const std::vector<LabeledSensorInfo>& sensorsInfo)
    {
        using namespace testing;
        std::vector<std::shared_ptr<NiceMock<SensorMock>>> result;
        for (const auto& sensorInfo : sensorsInfo)
        {
            auto& sensorMock =
                result.emplace_back(std::make_shared<NiceMock<SensorMock>>());
            ON_CALL(*sensorMock, getLabeledSensorInfo())
                .WillByDefault(Return(sensorInfo));
        }
        return utils::convContainer<std::shared_ptr<interfaces::Sensor>>(
            result);
    }

    MOCK_METHOD(Id, id, (), (const, override));
    MOCK_METHOD(std::string, metadata, (), (const, override));
    MOCK_METHOD(std::string, getName, (), (const, override));
    MOCK_METHOD(void, registerForUpdates,
                (const std::weak_ptr<interfaces::SensorListener>&), (override));
    MOCK_METHOD(void, unregisterFromUpdates,
                (const std::weak_ptr<interfaces::SensorListener>&), (override));
    MOCK_METHOD(LabeledSensorInfo, getLabeledSensorInfo, (), (const, override));

    const uint64_t mockId = generateUniqueMockId();

    Id mockSensorId = Id("SensorMock", "", "");

  private:
    void initialize()
    {
        ON_CALL(*this, id()).WillByDefault(testing::Invoke([this] {
            return this->mockSensorId;
        }));
    }
};

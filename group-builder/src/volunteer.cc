#include "volunteer.hpp"
/*Initializes a Volunteer object. If either building_experience
or physical_stamina is below the minimum allowed, set the value
to that minimum instead. If either building_experience or
physical_stamina is above the maximum allowed, set the value to
that maximum instead. See constants at the top of volunteer.hpp
for the max and mins allowed*/
Volunteer::Volunteer(const std::string& name,
                     int building_experience,
                     int physical_stamina,
                     bool returning):
    name_(name),
    building_experience_(std::min(
        std::max(building_experience, kMinBuildingExp), kMaxBuildingExp)),
    physical_stamina_(
        std::min(std::max(physical_stamina, kMinStamina), kMaxStamina)),
    returning_(returning) {}

std::string Volunteer::GetName() const { return name_; }

int Volunteer::GetBuildingExperience() const { return building_experience_; }

int Volunteer::GetPhysicalStamina() const { return physical_stamina_; }

bool Volunteer::IsReturning() const { return returning_; }
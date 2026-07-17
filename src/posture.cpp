#include "posture.h"
#include "config.h"

DistancePatternClassifier::DistancePatternClassifier(HCSR04* topSensor, HCSR04* sideSensor)
    : _topSensor(topSensor), _sideSensor(sideSensor), _lastPattern(DISTANCE_UNKNOWN) {
}

DistancePattern DistancePatternClassifier::classify() {
    const float topDist = _topSensor == nullptr ? -1 : _topSensor->getDistance();
    const float sideDist = _sideSensor == nullptr ? -1 : _sideSensor->getDistance();
    const bool topValid = topDist > 0 && topDist < 200;
    const bool sideValid = sideDist > 0 && sideDist < 200;

    if (!topValid && !sideValid) {
        _lastPattern = DISTANCE_UNKNOWN;
        return _lastPattern;
    }

    // These combinations are intentionally neutral names. They must not be
    // interpreted as a person, body, sleep or safety classification.
    if (topValid && sideValid) {
        if (topDist > TOP_DISTANCE_REFERENCE_MIN && sideDist < SIDE_DISTANCE_NEAR_THRESHOLD) {
            _lastPattern = DISTANCE_NEAR_THRESHOLD;
        } else if (topDist < TOP_DISTANCE_NEAR_MAX && sideDist > SIDE_DISTANCE_FAR_MIN) {
            _lastPattern = DISTANCE_SIDE_PATTERN;
        } else if (topDist > TOP_DISTANCE_REFERENCE_MIN && sideDist < SIDE_DISTANCE_REFERENCE_MAX) {
            _lastPattern = DISTANCE_REFERENCE_PATTERN;
        } else {
            _lastPattern = DISTANCE_AMBIGUOUS;
        }
    } else {
        _lastPattern = DISTANCE_UNKNOWN;
    }
    return _lastPattern;
}

float DistancePatternClassifier::getTopDistance() {
    return _topSensor == nullptr ? -1 : _topSensor->getDistance();
}

float DistancePatternClassifier::getSideDistance() {
    return _sideSensor == nullptr ? -1 : _sideSensor->getDistance();
}

const char* DistancePatternClassifier::getPatternString() const {
    switch (_lastPattern) {
        case DISTANCE_REFERENCE_PATTERN: return "reference_pattern";
        case DISTANCE_SIDE_PATTERN: return "side_pattern";
        case DISTANCE_AMBIGUOUS: return "ambiguous";
        case DISTANCE_NEAR_THRESHOLD: return "near_threshold";
        case DISTANCE_UNKNOWN: return "unknown";
        default: return "unknown";
    }
}

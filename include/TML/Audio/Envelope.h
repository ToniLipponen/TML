#pragma once
#include <TML/Export.h>

namespace tml
{
    class TML_API Envelope
    {
    public:
        Envelope() noexcept;
        Envelope(double attack, double decay, double sustain, double release) noexcept;
        Envelope(const Envelope&) noexcept = default;
        Envelope(Envelope&&) noexcept = default;
        virtual ~Envelope() noexcept = default;

        Envelope& operator=(const Envelope&) noexcept = default;
        Envelope& operator=(Envelope&&) noexcept = default;

        void SetAttack(double a) noexcept;
        void SetDecay(double d) noexcept;
        void SetSustain(double s) noexcept;
        void SetRelease(double r) noexcept;
        void SetOn(double time) noexcept;
        void SetOff(double time) noexcept;
        void Reset() noexcept;
        [[nodiscard]] bool GetOn() const noexcept;
        [[nodiscard]] double GetOutput(double time) const noexcept;
        [[nodiscard]] double GetAttack() const noexcept;
        [[nodiscard]] double GetDecay() const noexcept;
        [[nodiscard]] double GetSustain() const noexcept;
        [[nodiscard]] double GetRelease() const noexcept;

    private:
        double m_attack;
        double m_decay;
        double m_sustain;
        double m_release;
        double m_timeOn;
        double m_timeOff;
    };
}
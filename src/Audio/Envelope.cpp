#include <TML/Audio/Envelope.h>

namespace tml
{
    Envelope::Envelope() noexcept
    : m_attack(1), m_decay(1), m_sustain(1), m_release(1), m_timeOn(0), m_timeOff(m_timeOn)
    {

    }

    Envelope::Envelope(double attack, double decay, double sustain, double release) noexcept
    : m_attack(attack), m_decay(decay), m_sustain(sustain), m_release(release), m_timeOn(0), m_timeOff(m_timeOn)
    {

    }

    void Envelope::SetAttack(double a) noexcept
    {
        m_attack = a;
    }

    void Envelope::SetDecay(double d) noexcept
    {
        m_decay = d;
    }

    void Envelope::SetSustain(double s) noexcept
    {
        m_sustain = s;
    }

    void Envelope::SetRelease(double r) noexcept
    {
        m_release = r;
    }

    void Envelope::SetOn(double time) noexcept
    {
        m_timeOn = time;
    }

    void Envelope::SetOff(double time) noexcept
    {
        m_timeOff = time;
    }

    void Envelope::Reset() noexcept
    {
        m_timeOn = 0;
        m_timeOff = m_timeOn;
    }

    bool Envelope::GetOn() const noexcept
    {
        return m_timeOn > m_timeOff;
    }

    double Envelope::GetOutput(double time) const noexcept
    {
        double amplitude;
        double lifeTime = time - m_timeOn;
        double startAmplitude = 1.0;

        if(m_timeOn > m_timeOff)
        {
            if(lifeTime <= m_attack)
            {
                amplitude = (lifeTime / m_attack) * startAmplitude;
            }

            if(lifeTime > m_attack && lifeTime <= (m_attack + m_decay))
            {
                amplitude = ((lifeTime - m_attack) / m_decay) * (m_sustain - startAmplitude) + startAmplitude;
            }

            if(lifeTime > (m_attack + m_decay))
            {
                amplitude = m_sustain;
            }
        }
        else
        {
            double rAmplitude = 0;
            lifeTime = m_timeOff - m_timeOn;

            if (lifeTime <= m_attack)
                rAmplitude = (lifeTime / m_attack) * startAmplitude;

            if (lifeTime > m_attack && lifeTime <= (m_attack + m_decay))
                rAmplitude = ((lifeTime - m_attack) / m_decay) * (m_sustain - startAmplitude) + startAmplitude;

            if (lifeTime > (m_attack + m_decay))
                rAmplitude = m_sustain;

            amplitude = ((time - m_timeOff) / m_release) * (0.0 - rAmplitude) + rAmplitude;
        }

        if(amplitude <= 0.00001)
        {
            amplitude = 0;
        }

        return amplitude;
    }

    double Envelope::GetAttack() const noexcept
    {
        return m_attack;
    }

    double Envelope::GetDecay() const noexcept
    {
        return m_decay;
    }

    double Envelope::GetSustain() const noexcept
    {
        return m_sustain;
    }

    double Envelope::GetRelease() const noexcept
    {
        return m_release;
    }
}
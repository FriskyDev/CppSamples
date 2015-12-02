#pragma once

namespace chuckeng
{
    template <typename F>
    struct FinalAction
    {
        FinalAction(F f) : _action{ f } {}
        ~FinalAction()
        {
            if (_enabled) {
                _action();
            }
        }

        void Disable()
        {
            _enabled = false;
        };

    private:
        F _action;
        bool _enabled{ true };
    };
}

template <typename F>
chuckeng::FinalAction<F> finally(F f)
{
    return chuckeng::FinalAction<F>(f);
}


#ifndef ENTITY_h_
#define ENTITY_h_

struct IEntity
{
    virtual ~IEntity() = default;
    virtual void update(double delta) = 0;
    virtual void take_turn();
    int energy;
};

#endif
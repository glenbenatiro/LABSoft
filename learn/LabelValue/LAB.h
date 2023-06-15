#ifndef LAB_H
#define LAB_H

class LAB
{
  private:
    double m_value = 0.0;

  public:
    LAB ();

    double  value ();
    void    value (double val);
};

#endif
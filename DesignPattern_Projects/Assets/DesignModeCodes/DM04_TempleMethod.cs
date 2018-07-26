﻿using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace DM
{



    class DM04_TempleMethod : MonoBehaviour
    {
        void Start()
        {
            IPeople people = new SouthPeople();
            people.Eat();
        }
    }

    public abstract class IPeople
    {
        public void Eat()
        {
            Orderfoods();
            EatSomething();
            PayBill();
        }

        protected void Orderfoods()
        {
            BaseLog.Log("Order");
        }

        protected virtual void EatSomething() { }

        protected void PayBill()
        {
            BaseLog.Log("买单");
        }
    }

    public class NorthPeople : IPeople
    {
        protected override void EatSomething()
        {
            BaseLog.Log("面条");
        }
    }

    public class SouthPeople : IPeople
    {
        protected override void EatSomething()
        {
            BaseLog.Log("米饭");
        }
    }
}
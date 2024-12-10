using MonoEngine;
using System;

public class Player : MonoBehaviour
{
    private int _hp;

    void __internalAwake()
    {
        Console.WriteLine("Player::__internalAwake() called!");
    }

    void Awake()
    {
        _hp = 100;
        Console.WriteLine($"Player Awake! hp: {_hp}");
    }
    
    void Start()
    {
        var random = new Random();
        _hp += random.Next(10, 50);
        Console.WriteLine($"Player Start! hp: {_hp}");
    }

    void FixedUpdate()
    {
        if (Dead())
            return;

        _hp += 1;
        Console.WriteLine($"Player FixedUpdate, recover, hp: {_hp}");
    }

    void Update()
    {
        if (Dead())
            return;

        _hp -= 5;
        Console.WriteLine($"Player Update, take damage, hp: {_hp}");
    }

    private bool Dead()
    {
        return _hp <= 0;
    }
}
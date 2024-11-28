using MonoEngine;
using System;

public class Player : MonoBehaviour
{
    void __internalAwake()
    {
        Console.WriteLine("Player::__internalAwake() called!");
    }

    void Awake()
    {
        Console.WriteLine("Player Awake!");
    }
}
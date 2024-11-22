using MonoEngine;
using System;

public class Main
{
    static int CSharpInit()
    {
        var ob = new GameObject("Game");
        var gameOb = MonoEngine.GameObject.Find("Game");
        Console.WriteLine($"gameOb: {gameOb}, IsNull: {gameOb == null}");
        return 1;
    }
}
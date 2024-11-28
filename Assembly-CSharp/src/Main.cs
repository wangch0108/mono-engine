using MonoEngine;
using System;

public class Main
{
    static int CSharpInit()
    {
        var ob = new GameObject("Game");
        var gameOb = MonoEngine.GameObject.Find("Game");
        Console.WriteLine($"Find gameObject == new gameObject: {ob == gameOb}");
        Console.WriteLine($"gameOb: {gameOb}, IsNull: {gameOb == null}");

        var comp = gameOb.AddComponent<Player>();


        return 1;
    }
}
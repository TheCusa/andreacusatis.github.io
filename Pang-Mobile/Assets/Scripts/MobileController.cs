using UnityEngine;
using UnityEngine.EventSystems;

public class MobileController : MonoBehaviour, IPointerDownHandler, IPointerUpHandler

{
    public bool isPressed = false;

    //getting holding down input
    public void OnPointerDown(PointerEventData eventData)
    {
        isPressed = true;
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        isPressed = false;
    }
    
}

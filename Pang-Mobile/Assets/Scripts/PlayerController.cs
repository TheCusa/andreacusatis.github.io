using System.Data;
using System.Numerics;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using Vector3 = UnityEngine.Vector3;


public class PlayerController : MonoBehaviour

{
    enum Direction
    {
        Left,
        Right,
    }

    
    [SerializeField]
    MobileController upControl;
    [SerializeField]
    MobileController leftControl;
    [SerializeField]
    MobileController rightControl;
    [SerializeField]
    MobileController downControl;
    [SerializeField]
    Button fireControl;

    GameStatsHandler gameStatsHandler;

    public bool isShooting { get; private set; } = false;
    public bool canClimb { get;  set; } = false;


    public Animator animator = null;

    //private int Animator_Speed = Animator.StringToHash("SPEED");
    private int Animator_Walk = Animator.StringToHash("WALK");
    private int Animator_Dir = Animator.StringToHash("DIRECTION");
    private int Animator_Idle = Animator.StringToHash("IDLE");
    private int Animator_Shoot= Animator.StringToHash("IS_SHOOTING");
    public int Animator_Dead = Animator.StringToHash("DEAD");
   // public float Animator_FireRate = Animator.StringToHash("FIRE_RATE");
    private int Animator_isClimbing = Animator.StringToHash("IS_CLIMBING");

    Vector3 horizontalOffset;


    private bool isClimbing = false;

    private float horizontalMobile = 0;
    private float verticalMobile = 0;
    private bool fireMobile = false;
    private float currentMoveSpeed = 0;
    


    private void Awake()
    {
        animator= GetComponent<Animator>();
    }
    private void Start()
    {
        gameStatsHandler = FindAnyObjectByType<GameStatsHandler>();
        currentMoveSpeed = gameStatsHandler.gameStats.playerMoveSpeed;
    }
    private void OnEnable()
    {
       
       fireControl.onClick.AddListener(mobileFireControl);

        
    }
    private void OnDisable()
    {
        
        fireControl.onClick.RemoveListener(mobileFireControl);

        
    }

    // Update is called once per frame
    void Update()
    {
        
        //Animation handling
        if (animator != null)
        {
            mobileMoveControls();
            fireControl.interactable = false; //disabling fire control 

            //preventing player from moving left or right while climbing
            if (!isClimbing)
            {
                horizontalOffset = horizontalMovement(); 
            }
            //Checking if player can climb in order to enable vertical moving
            if (canClimb == true)
            {

                Vector3 verticalOffset = verticalMovement();
                if (verticalOffset.sqrMagnitude > Mathf.Epsilon)
                {
                    isClimbing = true;
                    animator.SetBool(Animator_isClimbing, isClimbing);
                }
                else
                {
                    isClimbing=false;
                    animator.SetBool(Animator_isClimbing, isClimbing);
                    
                }
            }
            //else
            //{
            //    animator.SetBool(Animator_isClimbing, false);

            //}

            //Setting walk and idle condition
            bool bWalk = horizontalOffset.sqrMagnitude > Mathf.Epsilon && !isClimbing && !isShooting? true : false;
            animator.SetBool(Animator_Walk, bWalk);


            bool idle = bWalk ||  isShooting || isClimbing ? false : true; 
            animator.SetBool(Animator_Idle, idle);


            if (bWalk)
            {
                Direction dir = Direction.Right;

                dir = horizontalOffset.x > Mathf.Epsilon ? Direction.Right : Direction.Left;
                animator.SetInteger(Animator_Dir, (int)dir);
            }

            if (!bWalk && !isClimbing)
            {
                fireControl.interactable = true; //enabling fire control
                isShooting = fire();             
                if (isShooting)
                {
                    animator.SetTrigger(Animator_Shoot);
                   fireMobile = false;   
                    
                }
            }
           
        }

    }

    
    //Hybrid movement (mobile + PC)
    Vector3 horizontalMovement()
    {
        float horizontal = (Input.GetAxis("Horizontal") + horizontalMobile) * currentMoveSpeed * Time.deltaTime;
        Vector3 offset = new Vector3(horizontal, 0, 0);                                 
        
        transform.position += offset;
        return offset;
    }
    Vector3 verticalMovement()
    {
        float vertical = (Input.GetAxis("Vertical") + verticalMobile) * currentMoveSpeed * Time.deltaTime;
        Vector3 offset = new Vector3(0, vertical, 0);
        transform.position += offset;
        return offset;
    }
    bool fire()
    {
        bool shoot = Input.GetKeyDown(KeyCode.Space) || fireMobile;

        return shoot;
    }
    //Mobile controls
    void mobileMoveControls()
    {
        //Checking which button is pressed and resetting mobile movement
         horizontalMobile = 0f;
         verticalMobile = 0f;

        if (leftControl != null && leftControl.isPressed)
        {
            horizontalMobile -= 1f;

        }

        else if (rightControl != null && rightControl.isPressed)
        {
            horizontalMobile += 1f;

        }

        if (upControl != null && upControl.isPressed)
        {
            verticalMobile += 1f;

        }
        else if (downControl != null && downControl.isPressed)
        {
            verticalMobile -= 1f;

        }
        
        
    }
    //function needed for OnEnable event
    void mobileFireControl()
    {
        fireMobile = true;
    }
        
    
    
}

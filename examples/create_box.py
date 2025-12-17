from mod3d.mod3d import create_box_summary, create_box


def main() -> None:
    summary = create_box_summary(45.0, 30.0, 18.0)
    print("Generated box summary:", summary)

    box = create_box(45.0, 30.0, 18.0)
    print("Generated box object:", box)


if __name__ == "__main__":
    main()

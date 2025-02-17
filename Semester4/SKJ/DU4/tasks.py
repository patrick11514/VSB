import xml.etree.ElementTree as ET


def get_student_by_id(xml_root: ET.Element, student_id: str):
    return xml_root.find(".//student[@student_id='" + student_id + "']")


def create_student(xml_root: ET.Element, student_id: str):
    """
    Vytvořte studenta dle loginu.
    Ujistěte se, že student neexistuje, jinak: raise Exception('student already exists')
    """

    founded = get_student_by_id(xml_root, student_id)

    if founded is not None:
        raise Exception("student already exists")

    xml_root.append(ET.Element("student", {"student_id": student_id}))


def remove_student(xml_root: ET.Element, student_id: str):
    """
    Odstraňte studenta dle loginu
    """

    founded = get_student_by_id(xml_root, student_id)
    if founded is None:
        return

    xml_root.remove(founded)


def get_task_by_id(student_root: ET.Element, task_id: str | int):
    return student_root.find(".//task[@task_id='" + str(task_id) + "']")


def set_task_points(
    xml_root: ET.Element, student_id: str, task_id: int | str, points: int | str
):
    """
    Přepište body danému studentovi u jednoho tasku
    """

    student = get_student_by_id(xml_root, student_id)
    if student is None:
        return

    task = get_task_by_id(student, task_id)

    if task is None:
        return

    task.text = str(points)


def create_task(
    xml_root: ET.Element, student_id: str, task_id: int | str, points: int | str
):
    """
    Pro daného studenta vytvořte task s body.
    Ujistěte se, že task (s task_id) u studenta neexistuje, jinak: raise Exception('task already exists')
    """

    student = get_student_by_id(xml_root, student_id)
    if student is None:
        return

    founded = get_task_by_id(student, task_id)

    if founded is not None:
        raise Exception("task already exists")

    task = ET.Element("task", {"task_id": task_id})
    task.text = str(points)
    student.append(task)


def remove_task(xml_root: ET.Element, task_id: str | int):
    """
    Napříč všemi studenty smažte task s daným task_id
    """

    for student in xml_root:
        task = get_task_by_id(student, task_id)

        if task is not None:
            student.remove(task)
